import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SpaceLinkJoinComponent } from './component';

describe('SpaceLinkJoinComponent', () => {
  let component: SpaceLinkJoinComponent;
  let fixture: ComponentFixture<SpaceLinkJoinComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
    imports: [SpaceLinkJoinComponent]
})
    .compileComponents();

    fixture = TestBed.createComponent(SpaceLinkJoinComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
