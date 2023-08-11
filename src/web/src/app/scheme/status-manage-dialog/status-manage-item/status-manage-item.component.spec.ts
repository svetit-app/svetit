import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { StatusManageItemComponent } from './status-manage-item.component';

describe('StatusManageItemComponent', () => {
  let component: StatusManageItemComponent;
  let fixture: ComponentFixture<StatusManageItemComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ StatusManageItemComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(StatusManageItemComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
